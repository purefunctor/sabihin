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

  <Layout>
    {switch (url.path) {
     | [] => <Index />
     | ["not-found"] | _ => <NotFound />
     }}
  </Layout>;
};
