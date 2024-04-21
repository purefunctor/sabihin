[@react.component]
let make = () => {
  let serverUrl = ServerUrl.useServerUrl();
  let url = ReasonReactRouter.useUrl(~serverUrl?, ());
  switch (url.path) {
  | [] => <IndexPage />
  | ["register"] => <RegisterPage />
  | ["not-found"]
  | _ => <NotFoundPage />
  };
};
