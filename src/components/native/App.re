[@react.component]
let make = (~serverUrl: option(ReasonReactRouter.url)=?) => {
  let url =
    switch%platform (Runtime.platform) {
    | Client => ReasonReactRouter.useUrl(~serverUrl?, ())
    | Server =>
      switch (serverUrl) {
      | Some(serverUrl) => serverUrl
      | None => raise(Invalid_argument("Server component needs URL!"))
      }
    };

  switch (url.path) {
  | [] => <div> {React.string("Index!")} </div>
  | [name] => <Counter name />
  | _ => <div> {React.string("Not Found!")} </div>
  };
};
