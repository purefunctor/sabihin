type hook('a) = {
  get: unit => 'a,
  set: 'a => unit,
  subscribe: (unit => unit, unit) => unit,
};

module type StoreKind = {
  type t;

  let use: unit => hook(t);
};

module MakeContext = (StoreKind: StoreKind) => {
  let context: React.Context.t(option(hook(StoreKind.t))) =
    React.createContext(None);

  let useContext = () => {
    switch (React.useContext(context)) {
    | Some(store) => store
    | None => failwith("Provider does not exist.")
    };
  };

  module Provider = {
    module Inner = {
      include React.Context;
      let make = provider(context);
    };

    [@react.component]
    let make = (~children) => {
      let value = Some(StoreKind.use());
      <Inner value> children </Inner>;
    };
  };
};
