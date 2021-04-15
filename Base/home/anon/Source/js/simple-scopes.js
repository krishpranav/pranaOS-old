// it should return 'undefined'
function foo() {
    function bar() {
        var y = 6;
    }

    bar();
    return y;
}

console.log(foo());
