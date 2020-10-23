import spritely from "./spritely.js";

const {
    loadSpritely,
    registerDrawFn,
    registerUpdateFn,
} = spritely();

/* loads spritely canvas on screen */
loadSpritely().then(init)

function init() {
    // set_app_state(app_states.GAME)
    registerDrawFn(render);
    registerUpdateFn(update);
}

function update() {
}

function render() {
}
