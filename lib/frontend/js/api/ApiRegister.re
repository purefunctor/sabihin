open ApiCore;
open Promise_syntax;
open Types_js.Definitions_bs;

let register = (payload: register_user_payload): Js.Promise.t(registerResult) => {
  open Js.Promise;

  let body =
    write_register_user_payload(payload)
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
      tryParse(read_register_response, registerJson)
    | (_, Some(registerJson)) =>
      tryParseError(read_register_error_response, registerJson)
    | _ => Error(`CouldNotParse)
    };

  resolve(registerResult);
};
