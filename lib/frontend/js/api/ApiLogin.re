open ApiCore;
open Promise_syntax;
open Types_js.Definitions_bs;

let postCommon = (payload: login_payload): Js.Promise.t(Fetch.Response.t) => {
  let body =
    write_login_payload(payload) |> Js.Json.stringify |> Fetch.BodyInit.make;
  let method_ = Fetch.Post;
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  let requestInit = Fetch.RequestInit.make(~method_, ~headers, ~body, ());
  Fetch.fetchWithInit("/api/login", requestInit);
};

let postSalt = (~username: string): Js.Promise.t(loginSaltResult) => {
  open Js.Promise;

  let payload: login_payload = {username, auth_token: None};
  let* loginResponse = postCommon(payload);

  let loginStatus = Fetch.Response.status(loginResponse);
  let* loginJson =
    Fetch.Response.json(loginResponse)
    |> then_(loginJson => resolve(Some(loginJson)))
    |> catch(_ => resolve(None));

  let loginResult =
    switch (loginStatus, loginJson) {
    | (200, Some(loginJson)) =>
      tryParse(read_login_salt_response, loginJson)
    | (_, Some(loginJson)) =>
      tryParseError(read_login_error_response, loginJson)
    | _ => Error(`CouldNotParse)
    };

  resolve(loginResult);
};

let postAuth =
    (~username: string, ~auth_token: string): Js.Promise.t(loginAuthResult) => {
  open Js.Promise;

  let payload: login_payload = {username, auth_token: Some(auth_token)};
  let* loginResponse = postCommon(payload);

  let loginStatus = Fetch.Response.status(loginResponse);
  let* loginJson =
    Fetch.Response.json(loginResponse)
    |> then_(loginJson => resolve(Some(loginJson)))
    |> catch(_ => resolve(None));

  let loginResult =
    switch (loginStatus, loginJson) {
    | (204, None) => Ok()
    | (_, Some(loginJson)) =>
      tryParseError(read_login_error_response, loginJson)
    | _ => Error(`CouldNotParse)
    };

  resolve(loginResult);
};
