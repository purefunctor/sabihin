open Types_js;
open Types_universal;

let readSession = () => {
  let ( let* ) = Option.bind;
  let kind = {
    let* kindString = Dom.Storage.(localStorage |> getItem("sessionKind"));
    let* kindJson = Json.parse(kindString);
    switch (Definitions_bs.read_session_kind(kindJson)) {
    | kind => Some(kind)
    | exception _ => None
    };
  };
  Js.Promise.resolve(
    switch (kind) {
    | Some(kind) => kind
    | None => `Guest
    },
  );
};

let writeSession = kind => {
  let kindJson = Definitions_bs.write_session_kind(kind);
  let kindString = Json.stringify(kindJson);
  Dom.Storage.(localStorage |> setItem("sessionKind", kindString));
  Js.Promise.resolve();
};

include Store.MakeContext({
  open Store;

  type t = Definitions_t.session_kind;

  let use = () => {
    let ( let* ) = (f, x) => Js.Promise.then_(x, f);

    let subscribers = React.useRef(Subscribers_js.create());
    let get = readSession;
    let set = kind => {
      let* _ = writeSession(kind);
      Subscribers_js.forEach(subscribers.current, subscriber => subscriber());
      Js.Promise.resolve();
    };
    let subscribe = callback => {
      let key = Subscribers_js.add(subscribers.current, callback);
      () => {
        Subscribers_js.delete(subscribers.current, key);
      };
    };
    {get, set, subscribe};
  };
});
