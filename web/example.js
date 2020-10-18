import spritely from "./spritely.js";

const _engine = spritely();

/* loads spritely canvas on screen */
_engine.loadSpritely().then(game)

function game() {
    /* register gameloop functions */
    // render_testing()
}

function render_testing() {
    _engine.set_app_state(_engine.app_states.GAME)
    _engine.render_char('H'.charCodeAt(0), 100, 100)
    _engine.render_char('E'.charCodeAt(0), 120, 100)
    _engine.render_char('L'.charCodeAt(0), 140, 100)
    _engine.render_char('L'.charCodeAt(0), 160, 100)
    _engine.render_char('O'.charCodeAt(0), 180, 100)
    _engine.render_char(','.charCodeAt(0), 200, 100)
    _engine.render_char('W'.charCodeAt(0), 100, 120)
    _engine.render_char('O'.charCodeAt(0), 120, 120)
    _engine.render_char('R'.charCodeAt(0), 140, 120)
    _engine.render_char('L'.charCodeAt(0), 160, 120)
    _engine.render_char('D'.charCodeAt(0), 180, 120)
    
    _engine.render_string("hello, world!", 0, 0);
}
