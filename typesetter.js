const bindings = require('bindings');
const typesetter = bindings('typesetter.node');

const KEYS = Object.freeze({
  SPACE: 57,
  LEFT: 105,
});

module.exports = { typesetter, KEYS };
