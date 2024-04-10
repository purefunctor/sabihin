open SessionTypes;
open Types_js.Defs_bs;

module API = {
  type generalError('apiError) = [ | `CouldNotParse | `ApiError('apiError)];

  type registerError = generalError(register_error_response_t);
  type registerResult = result(register_response_t, registerError);

  let tryParse = (parser, json) =>
    switch (parser(json)) {
    | result => Ok(result)
    | exception _ => Error(`CouldNotParse)
    };

  let tryParseError = (parser, json) => {
    switch (parser(json)) {
    | result => Error(`ApiError(result))
    | exception _ => Error(`CouldNotParse)
    };
  };

  let register = (payload: register_payload_t): Js.Promise.t(registerResult) => {
    open Js.Promise;

    let ( let* ) = (f, x) => then_(x, f);

    let body =
      write_register_payload_t(payload)
      |> Js.Json.stringify
      |> Fetch.BodyInit.make;

    let* registerResponse = {
      let method_ = Fetch.Post;
      let headers =
        Fetch.HeadersInit.make({"Content-Type": "application/json"});
      let requestInit = Fetch.RequestInit.make(~method_, ~headers, ~body, ());
      Fetch.fetchWithInit("/api/register", requestInit);
    };
    let* registerJson =
      Fetch.Response.json(registerResponse)
      |> then_(registerJson => resolve(Some(registerJson)))
      |> catch(_ => resolve(None));

    let registerResult =
      switch (Fetch.Response.status(registerResponse), registerJson) {
      | (200, Some(registerJson)) =>
        tryParse(read_register_response_t, registerJson)
      | (_, Some(registerJson)) =>
        tryParseError(read_register_error_response_t, registerJson)
      | _ => Error(`CouldNotParse)
      };

    resolve(registerResult);
  };
};

let useSession = () => {
  let store = SessionStore.useContext();

  // The default session state is always the guest.
  let (session, setSession) = React.useState(() => Guest);

  React.useEffect0(() => {
    let unsubscribe = store.subscribe(() => setSession(_ => store.get()));
    // We introduce artificial latency here by setting the local session
    // state to Loading, then reading from the SessionStore after 500ms.
    // In the future, readSession may involve calling into the network.
    setSession(_ => Loading);
    let timeoutId =
      Js.Global.setTimeout(
        ~f=() => store.set(SessionStore.readSession()),
        500,
      );
    Some(
      () => {
        unsubscribe();
        Js.Global.clearTimeout(timeoutId);
      },
    );
  });

  session;
};

let useRegister = () => {
  let store = SessionStore.useContext();
  React.useCallback0(payload => {
    let ( let* ) = (f, x) => Js.Promise.then_(x, f);
    let original = store.get();
    store.set(Loading);
    let* registerResult = API.register(payload);
    switch (registerResult) {
    | Ok(registerResult) =>
      store.set(LoggedIn({publicId: registerResult.public_id}))
    | Error(_) => store.set(original)
    };
    Js.Promise.resolve(registerResult);
  });
};
