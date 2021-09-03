const { typesetter, KEYS } = require('./typesetter');

typesetter.init();
setTimeout(() => {
  typesetter.type(new Int32Array([KEYS.SPACE, KEYS.LEFT]));
}, 1000);
