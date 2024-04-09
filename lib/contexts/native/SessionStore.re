open SessionTypes;

let context: React.Context.t(option(store)) = React.createContext(None);

module Provider = {
  module Inner = {
    include React.Context;
    let make = React.Context.provider(context);
  };

  [@react.component]
  let make = (~children) => {
    <Inner value=None> children </Inner>;
  };
};
