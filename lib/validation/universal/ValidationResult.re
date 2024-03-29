type t('a) =
  | Validated('a)
  | NotValidated;

let map_or = (default: 'b, f: 'a => 'b, validated: t('a)) => {
  switch (validated) {
  | Validated(v) => f(v)
  | NotValidated => default
  };
};

let map_or_none = (f: 'a => 'b, validated: t('a)) => {
  map_or(None, v => Some(f(v)), validated);
};
