/**
 * A cheap implementation of pong
 * has some bugs
 */

import spritely from "../spritely.js";

const {
    loadSpritely,
    set_app_state,
    app_states,
    registerDrawFn,
    registerUpdateFn,
    rectFill,
    btn,
    CONTROLLER_BTNS,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
} = spritely();

/* loads spritely canvas on screen */
loadSpritely().then(init)

const MOVE_INCREMENT = 5;

const player = {
    x: 20,
    y: 0,
    width: 20,
    height: 100,
    score: 0,
    move: () => {
        if (btn(CONTROLLER_BTNS.BTN_UP)) {
            if (player.y - MOVE_INCREMENT < 0) return;

            player.y -= MOVE_INCREMENT;
        }

        if (btn(CONTROLLER_BTNS.BTN_DOWN)) {
            if (player.y + player.height + MOVE_INCREMENT > SCREEN_HEIGHT) return;
            player.y += MOVE_INCREMENT;
        }
    },
    render: () => {
        rectFill(
            player.x,
            player.y,
            player.width,
            player.height,
            2);
    }
}

const enemy = {
    x: SCREEN_WIDTH - 40,
    y: 100,
    width: 20,
    height: 100,
    score: 0,
    move: () => {
        if (enemy.y + enemy.height < 0) return;

        if (ball.y > enemy.y) enemy.y+=3;
        if (ball.y < enemy.y) enemy.y-=3;

    },
    render: () => {
        rectFill(
            enemy.x,
            enemy.y,
            enemy.width,
            enemy.height,
            2);
    }
}

const ball = {
    x: SCREEN_WIDTH / 2,
    y: SCREEN_HEIGHT / 2,
    width: 10,
    height: 10,
    velocity: 1,
    reset: () => {
        ball.x = SCREEN_WIDTH / 2;
        ball.y = SCREEN_HEIGHT / 2;
    },
    move: () => {
        ball.y += ball.velocity;
        ball.x += ball.velocity;

        if (ball.x < 0) {
            ball.reset();
            enemy.score++;
            console.log(player.score, " : ", enemy.score);
        }
        if (ball.x > SCREEN_WIDTH) {
            ball.reset();
            player.score++;
            console.log(player.score, " : ", enemy.score);
        }
        if (ball.y < 0 || ball.y > SCREEN_HEIGHT) {
            ball.velocity = increaseVelocity(ball.velocity);
        }

        if (ball.x < player.x + player.width) {
            if (ball.y < player.y + player.height && ball.y > player.y) {
                ball.velocity = increaseVelocity(ball.velocity);
            }
        }
        if (ball.x > enemy.x) {
            if (ball.y < enemy.y + enemy.height && ball.y > enemy.y) {
                ball.velocity = increaseVelocity(ball.velocity);
            }
        }
    },
    render: () => {
        rectFill(
            ball.x,
            ball.y,
            ball.width,
            ball.height,
            2);
    }
}


/**
 * increaseVelocity
 *   - increases the velocity and reverses direction
 * @param {Float} currentVelocity 
 */
function increaseVelocity(currentVelocity) {
    if (Math.abs(currentVelocity) > 8) return currentVelocity;

    return currentVelocity * -1.1;
}

function init() {
    set_app_state(app_states.GAME)
    registerDrawFn(render);
    registerUpdateFn(update);
}

function update() {
    player.move();
    ball.move();
    enemy.move();
}

function render() {
    player.render();
    enemy.render();
    ball.render();
}
