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

let extractStore = store => {
  switch (store) {
  | Some(store) => store
  | None => failwith("Missing SessionStore.Provider!")
  };
};

let useSession = () => {
  let store = React.useContext(SessionStore.context) |> extractStore;

  let (session, setSession) = React.useState(() => store.get());

  React.useEffect1(
    () => {Some(store.subscribe(() => setSession(_ => store.get())))},
    [||],
  );

  let register =
    React.useCallback1(
      payload => {
        let ( let* ) = (f, x) => Js.Promise.then_(x, f);
        let* registerResult = API.register(payload);
        switch (registerResult) {
        | Ok(registerResult) =>
          store.set(LoggedIn({publicId: registerResult.public_id}))
        | Error(_) => ()
        };
        Js.Promise.resolve(registerResult);
      },
      [||],
    );

  (session, register);
};
