[@react.component]
let make = () => {
  let serverUrl = ServerUrl.useServerUrl();
  let url = ReasonReactRouter.useUrl(~serverUrl?, ());
  switch (url.path) {
  | [] => <IndexPage />
  | ["register"] => <RegisterPage />
  | ["login"] => <LoginPage />
  | ["get-started"] => <GetStartedPage />
  | ["inbox"] => <InboxPage />
  | ["not-found"]
  | _ => <NotFoundPage />
  };
};
