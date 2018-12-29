// Developing purposes only
function debug(value, json = false) {
  const chalk = require('chalk');
  console.log(chalk.cyan("====================================================================="));

  if (json) {
    console.log(chalk.cyan(JSON.stringify(value, null, 4)));
  }
  else {
    console.log(chalk.cyan(value));
  }
  console.log(chalk.cyan("====================================================================="));
  console.log(chalk.red('Press the Enter key to continue.'));
  require('child_process').spawnSync("read _ ", {shell: true, stdio: [0, 1, 2]});
}

module.exports = debug;