open Promise_syntax;
open Types_js.Definitions_bs;

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
