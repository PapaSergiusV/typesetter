const bindings = require('bindings');
const typesetter = bindings('typesetter.node');

const KEYS = Object.freeze({
  SPACE: 57,
  LEFT: 105,
  C: 46,
});

module.exports = { typesetter, KEYS };
