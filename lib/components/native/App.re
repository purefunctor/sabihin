module Content = {
  [@react.component]
  let make = (~serverUrl: option(ReasonReactRouter.url)) => {
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
    | [] => <Index />
    | ["login"] => <Login />
    | ["register"] => <Register />
    | ["profile"] => <Profile />
    | ["not-found"]
    | _ => <NotFound />
    };
  };
};

[@react.component]
let make = (~serverUrl: option(ReasonReactRouter.url)=?) => {
  <SessionStore.Provider>
    <Layout> <Content serverUrl /> </Layout>
  </SessionStore.Provider>;
};
