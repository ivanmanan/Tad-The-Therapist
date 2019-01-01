// https://github.com/Marak/say.js/

const say = require('say');
const prompt = require('prompt');

//say.speak("Hello!");

prompt.start();

startSpeak();

function startSpeak() {
    prompt.get(['message'], (error, result) => {
        say.speak(result.message, 'Microsoft Zira Desktop', 1, () => {
            if (result.message == "goodbye") {
                process.exit();
            }
            startSpeak();
        });
    });
}