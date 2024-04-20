let context: React.Context.t(option(ReasonReactRouter.url)) =
  React.createContext(None);

module Provider = {
  include React.Context;
  let make = React.Context.provider(context);
};

let useServerUrl = () => {
  React.useContext(context);
};
