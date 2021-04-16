// figure out a way to document non-function entities too.
class Position {
    constructor(column, row, sheet) {
        this.column = column;
        this.row = row;
        this.sheet = sheet ?? this.Sheet;
        this.name = `${column}${row}`;
    }

    get contents() {
        return this.sheet.get_real_cell_contents(this.name)
    }

    set contents(value) {
        value = `${value}`;
        this.sheet.set_real_cell_contents(this.name, value);
        return value;
    }

    static from_name(name) {
        let sheet = thisSheet;
        let obj = sheet.parse_cell_name(name);
        return new Position(obj.column, obj.row, sheet);
    }

    up(how_many) {
        how_many = how_many ?? 1;
        const row = Math.max(0, this.row - how_many);
        return new Position(this.column, row, thi.sheet);
    }

    down(how_many) {
        how_many = how_many ?? 1;
        const row = Math.max(0, this.row + how_many);
        return new Position(this.column, row, this.sheet);
    }

    left(how_many) {
        how_many = how_many ?? 1;
        return new Position(
            this.sheet.column_arithmetic(this.column, -how_many),
            this.row,
            this.sheet
        );
    }
}