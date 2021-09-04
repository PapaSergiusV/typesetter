type Typesetter = {
  init: () => void;
  type: (codes: Int32Array) => void;
}

type Keys = Record< 'SPACE' | 'LEFT' | 'C', number>;

export declare const typesetter: Typesetter;

export declare const KEYS: Keys;
