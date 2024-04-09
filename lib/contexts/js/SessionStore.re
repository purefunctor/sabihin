open SessionTypes;

let readSession = () => {
  let ( let* ) = Option.bind;
  let loggedIn = {
    let* publicId = Dom.Storage.(localStorage |> getItem("publicId"));
    Some(LoggedIn({publicId: publicId}));
  };
  switch (loggedIn) {
  | Some(loggedIn) => loggedIn
  | None => Guest
  };
};

let writeSession = kind => {
  switch (kind) {
  | Loading => ()
  | Guest => ()
  | LoggedIn({publicId}) =>
    Dom.Storage.(localStorage |> setItem("publicId", publicId))
  };
};

let useStore = () => {
  let subscribers = React.useRef(Subscribers_js.create());
  let get = () => {
    readSession();
  };
  let set = kind => {
    writeSession(kind);
    Subscribers_js.forEach(subscribers.current, subscriber => {subscriber()});
  };
  let subscribe = callback => {
    let key = Subscribers_js.add(subscribers.current, callback);
    () => {
      Subscribers_js.delete(subscribers.current, key);
    };
  };
  {get, set, subscribe};
};

let context: React.Context.t(option(store)) = React.createContext(None);

module Provider = {
  module Inner = {
    include React.Context;
    let make = React.Context.provider(context);
  };

  [@react.component]
  let make = (~children) => {
    let store = useStore();
    <Inner value={Some(store)}> children </Inner>;
  };
};
