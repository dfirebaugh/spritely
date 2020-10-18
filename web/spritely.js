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

    window.SPRITELY = {
        ps,
        render_char,
        set_app_state,
        new_entity_manager,
        clear_entity_manager,
        add_sprite_to_entity_manager
    }
}

const app_states = {
    REPL: 0,
    SPRITE_EDITOR: 1,
    GAME: 2
}

function print_something() {
    SPRITELY.ps();
}

function render_char(char, x, y) {
    SPRITELY.render_char(char, x, y);
}

function set_app_state(state) {
    SPRITELY.set_app_state(state);
}

function render_string(str, x, y) {
    Array.from(str).forEach((char, index) => 
        SPRITELY.render_char(char.charCodeAt(0), (index*20) + x, y))
}

export default function() {
    return {
        loadSpritely,
        init: cb => cb(),
        ps: print_something,
        render_char,
        set_app_state,
        app_states,
        render_string
    }
}
