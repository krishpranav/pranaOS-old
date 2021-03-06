// includes
#include "RemoteProcess.h"
#include "RemoteObject.h"
#include "RemoteObjectGraphModel.h"
#include "RemoteObjectPropertyModel.h"
#include <stdio.h>
#include <stdlib.h>

namespace Inspector {

RemoteProcess* s_the;

RemoteProcess& RemoteProcess::the()
{
    return *s_the;
}

RemoteProcess::RemoteProcess(pid_t pid)
    : m_pid(pid)
    , m_object_graph_model(RemoteObjectGraphModel::create(*this))
    , m_socket(Core::LocalSocket::construct())
{
    s_the = this;
    m_socket->set_blocking(true);
}

void RemoteProcess::handle_identify_response(const JsonObject& response)
{
    int pid = response.get("pid").to_int();
    VERIFY(pid == m_pid);

    m_process_name = response.get("process_name").as_string_or({});

    if (on_update)
        on_update();
}

void RemoteProcess::handle_get_all_objects_response(const JsonObject& response)
{
    // FIXME: It would be good if we didn't have to make a local copy of the array value here!
    auto objects = response.get("objects");
    auto& object_array = objects.as_array();

    NonnullOwnPtrVector<RemoteObject> remote_objects;
    HashMap<FlatPtr, RemoteObject*> objects_by_address;

    for (auto& value : object_array.values()) {
        VERIFY(value.is_object());
        auto& object = value.as_object();
        auto remote_object = make<RemoteObject>();
        remote_object->address = object.get("address").to_number<FlatPtr>();
        remote_object->parent_address = object.get("parent").to_number<FlatPtr>();
        remote_object->name = object.get("name").to_string();
        remote_object->class_name = object.get("class_name").to_string();
        remote_object->json = object;
        objects_by_address.set(remote_object->address, remote_object);
        remote_objects.append(move(remote_object));
    }

    for (size_t i = 0; i < remote_objects.size(); ++i) {
        auto& remote_object = remote_objects.ptr_at(i);
        auto* parent = objects_by_address.get(remote_object->parent_address).value_or(nullptr);
        if (!parent) {
            m_roots.append(move(remote_object));
        } else {
            remote_object->parent = parent;
            parent->children.append(move(remote_object));
        }
    }

    m_object_graph_model->update();

    if (on_update)
        on_update();
}

void RemoteProcess::send_request(const JsonObject& request)
{
    auto serialized = request.to_string();
    i32 length = serialized.length();
    m_socket->write((const u8*)&length, sizeof(length));
    m_socket->write(serialized);
}

void RemoteProcess::set_inspected_object(FlatPtr address)
{
    JsonObject request;
    request.set("type", "SetInspectedObject");
    request.set("address", address);
    send_request(request);
}

void RemoteProcess::set_property(FlatPtr object, const StringView& name, const JsonValue& value)
{
    JsonObject request;
    request.set("type", "SetProperty");
    request.set("address", object);
    request.set("name", JsonValue(name));
    request.set("value", value);
    send_request(request);
}

void RemoteProcess::update()
{
    m_socket->on_connected = [this] {
        dbgln("Connected to PID {}", m_pid);

        {
            JsonObject request;
            request.set("type", "Identify");
            send_request(request);
        }

        {
            JsonObject request;
            request.set("type", "GetAllObjects");
            send_request(request);
        }
    };

    m_socket->on_ready_to_read = [this] {
        if (m_socket->eof()) {
            dbgln("Disconnected from PID {}", m_pid);
            m_socket->close();
            return;
        }

        u32 length;
        int nread = m_socket->read((u8*)&length, sizeof(length));
        if (nread != sizeof(length)) {
            dbgln("Disconnected from PID {}", m_pid);
            m_socket->close();
            return;
        }

        ByteBuffer data;
        size_t remaining_bytes = length;

        while (remaining_bytes) {
            auto packet = m_socket->read(remaining_bytes);
            if (packet.size() == 0)
                break;
            data.append(packet.data(), packet.size());
            remaining_bytes -= packet.size();
        }

        VERIFY(data.size() == length);
        dbgln("Got data size {} and read that many bytes", length);

        auto json_value = JsonValue::from_string(data);
        VERIFY(json_value.has_value());
        VERIFY(json_value.value().is_object());

        dbgln("Got JSON response {}", json_value.value());

        auto& response = json_value.value().as_object();

        auto response_type = response.get("type").as_string_or({});
        if (response_type.is_null())
            return;

        if (response_type == "GetAllObjects") {
            handle_get_all_objects_response(response);
            return;
        }

        if (response_type == "Identify") {
            handle_identify_response(response);
            return;
        }
    };

    auto success = m_socket->connect(Core::SocketAddress::local(String::formatted("/tmp/rpc/{}", m_pid)));
    if (!success) {
        warnln("Couldn't connect to PID {}", m_pid);
        exit(1);
    }
}

}
