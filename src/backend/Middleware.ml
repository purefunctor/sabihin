let csrf_cookie_name = "sabihin.csrf"

let issue_csrf_cookie inner_handler request =
  match Dream.cookie request csrf_cookie_name with
  | Some _ -> inner_handler request
  | None ->
      let%lwt inner_response = inner_handler request in
      let fresh_csrf_token = Dream.csrf_token request in
      Dream.set_cookie ~same_site:(Some `Strict) inner_response request
        csrf_cookie_name fresh_csrf_token;
      Lwt.return inner_response

let verify_csrf_cookie inner_handler request =
  match Dream.cookie request csrf_cookie_name with
  | Some csrf_token -> (
      let%lwt csrf_result = Dream.verify_csrf_token request csrf_token in
      match csrf_result with
      | `Ok -> inner_handler request
      | `Wrong_session | `Expired _ | `Invalid ->
          Dream.respond ~status:`Forbidden "Invalid credentials.")
  | None -> Dream.respond ~status:`Forbidden "Invalid credentials."
