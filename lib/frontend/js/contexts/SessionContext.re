open Promise_syntax;
open Client_types_bs;

let readSession = () => {
  let ( let* ) = Option.bind;
  let kind = {
    let* kindString = Dom.Storage.(localStorage |> getItem("sessionKind"));
    let* kindJson = Json.parse(kindString);
    switch (read_sessionKind(kindJson)) {
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
  let kindJson = write_sessionKind(kind);
  let kindString = Json.stringify(kindJson);
  Dom.Storage.(localStorage |> setItem("sessionKind", kindString));
  Js.Promise.resolve();
};

include Store.MakeContext({
  open Store;

  type t = sessionKind;

  let use = () => {
    let subscribers = React.useRef(Subscribers_js.create());
    let get = readSession;
    let set = kind => {
      let+ _ = writeSession(kind);
      Subscribers_js.forEach(subscribers.current, subscriber => subscriber());
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
