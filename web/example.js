import spritely from "./spritely.js";

const {
    loadSpritely,
    set_app_state,
    app_states,
    registerDrawFn,
    registerUpdateFn,
    rectFill,
    rect,
    btn
} = spritely();

/* loads spritely canvas on screen */
loadSpritely().then(init)


const rectangle = {
    x: 0,
    y: 0,
    width: 20,
    height: 20
}

function init() {
    set_app_state(app_states.GAME)
    registerDrawFn(render);
    registerUpdateFn(update);
}

function update() {
    if (btn(0))
        rectangle.x--;

    if (btn(1))
        rectangle.x++;

    if (btn(2))
        rectangle.y--;

    if (btn(3))
        rectangle.y++;

}

function render(){
    rectFill(
        rectangle.x,
        rectangle.y,
        rectangle.width,
        rectangle.height,
        2);
}
