[@react.component]
let make = () => {
  let serverUrl = ServerUrl.useServerUrl();
  let url = ReasonReactRouter.useUrl(~serverUrl?, ());
  switch (url.path) {
  | [] => <IndexPage />
  | ["register"] => <RegisterPage />
  | ["get-started"] => <GetStartedPage />
  | ["not-found"]
  | _ => <NotFoundPage />
  };
};
