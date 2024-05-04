open ApiCore;
open Promise_syntax;
open Types_js.Definitions_bs;

let get = (): Js.Promise.t(option(register_keys_payload)) => {
  open Js.Promise;

  let* response = {
    let method_ = Fetch.Get;
    let requestInit = Fetch.RequestInit.make(~method_, ());
    Fetch.fetchWithInit("/api/secrets", requestInit);
  };
  let* json =
    Fetch.Response.json(response)
    |> then_(json => resolve(Some(json)))
    |> catch(_ => resolve(None));

  let result =
    switch (Fetch.Response.status(response), json) {
    | (200, Some(json)) =>
      switch (tryParse(read_register_keys_payload, json)) {
      | Ok(json) => Some(json)
      | Error(_) => None
      }
    | (_, _) => None
    };

  resolve(result);
};

let post = (payload: register_keys_payload): Js.Promise.t(bool) => {
  let* response = {
    let body =
      payload
      |> write_register_keys_payload
      |> Js.Json.stringify
      |> Fetch.BodyInit.make;
    let method_ = Fetch.Post;
    let headers =
      Fetch.HeadersInit.make({"Content-Type": "application/json"});
    let requestInit = Fetch.RequestInit.make(~method_, ~headers, ~body, ());
    Fetch.fetchWithInit("/api/secrets", requestInit);
  };

  switch (Fetch.Response.status(response)) {
  | 204 => resolve(true)
  | _ => resolve(false)
  };
};
