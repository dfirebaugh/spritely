import Module from './index.js'
const spritely = Module({
    canvas: (function () { return document.getElementById('canvas'); })()
});

/**
 * loadSpritely - bind some c functions
 */
async function loadSpritely(){
    const instance = await spritely;
    const ps = instance.cwrap('print_something');
    const render_char = instance.cwrap("render_char", null, ["number", "number", "number"])
    const set_app_state = instance.cwrap("change_state", null, ["number"]);
    const new_entity_manager = instance.cwrap("new_entity_manager", "number");
    const clear_entity_manager = instance.cwrap("Entity_free", null, ["number"]);
    const add_sprite_to_entity_manager = instance.cwrap("add_sprite_to_entity_manager", null, ["number", "number", "number", "number"]);
    const registerDrawFn = instance.cwrap("register_draw_fn", null, ["number"]);
    const registerUpdateFn = instance.cwrap("register_update_fn", null, ["number"]);
    const clear = instance.cwrap("clear");
    const rect = instance.cwrap("render_rect", null, ["number", "number", "number", "number", "number"]);
    const rectFill = instance.cwrap("render_rect_fill", null, ["number", "number", "number", "number", "number"]);
    const color = instance.cwrap("set_render_color", null, ["number"]);
    const btn = instance.cwrap("is_btn_pressed", "number", ["number"]);

    window.SPRITELY = {
        addFunction: instance.addFunction,
        clear,
        instance,
        rect,
        rectFill,
        color,
        ps,
        registerDrawFn,
        registerUpdateFn,
        render_char,
        set_app_state,
        new_entity_manager,
        clear_entity_manager,
        btn,
        add_sprite_to_entity_manager
    }
}

const app_states = {
    REPL: 0,
    SPRITE_EDITOR: 1,
    GAME: 2
}

const CONTROLLER_BTNS = {
    LEFT: 0,
    RIGHT: 1,
    UP: 2,
    DOWN: 3,
    SECONDARY: 4,
    PRIMARY: 5,
    PAUSE: 6
}

function print_something() {
    SPRITELY.ps();
}

function render_char(char, x, y) {
    SPRITELY.render_char(char.charCodeAt(0), x, y);
}

function set_app_state(state) {
    SPRITELY.set_app_state(state);
}

function render_string(str, x, y) {
    Array.from(str).forEach((char, index) => 
        SPRITELY.render_char(char, (index*20) + x, y))
}

function registerDrawFn(drawFn) {
    const newFuncPtr = SPRITELY.addFunction(drawFn, "v")

    SPRITELY.registerDrawFn([newFuncPtr])
}

function registerUpdateFn(updateFn) {
    const newFuncPtr = SPRITELY.addFunction(updateFn, "v")

    SPRITELY.registerUpdateFn([newFuncPtr])
}

function rect(x, y, width, height, color) {
    SPRITELY.rect(x, y, width, height, color);
}

function rectFill(x, y, width, height, color) {
    SPRITELY.rectFill(x, y, width, height, color);
}

function clear() {
    SPRITELY.clear;
}

function color(new_color) {
    SPRITELY.color(new_color);
}

function btn(buttonToCheck) {
    return SPRITELY.btn(buttonToCheck);
}

export default function() {
    return {
        loadSpritely,
        ps: print_something,
        render_char,
        set_app_state,
        app_states,
        render_string,
        registerDrawFn,
        registerUpdateFn,
        clear,
        rect,
        rectFill,
        color,
        btn,
        CONTROLLER_BTNS,
        SCREEN_WIDTH: 670,
        SCREEN_HEIGHT: 625,
    }
}
