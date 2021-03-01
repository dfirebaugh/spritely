import spritely from "./spritely.js";

const {
    loadSpritely,
    registerDrawFn,
    registerUpdateFn,
    ps,
    render_char,
    set_app_state,
    app_states,
    render_string,
    clear,
    rect,
    rectFill,
    color,
    btn,
    CONTROLLER_BTNS,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
} = spritely();

/* loads spritely canvas on screen */
loadSpritely().then(init)

function init() {
    // set_app_state(app_states.GAME)
    registerDrawFn(render);
    registerUpdateFn(update);
}

function update() {
    if (btn(CONTROLLER_BTNS.UP))
        ps()
}

function render() {
    rectFill(0,0,25,25,3)
    rectFill(250,25,25,25,3)
    render_string("HELLO", 10, 10)
}
