let issue_csrf_cookie inner_handler request =
  match Dream.cookie request CSRF.cookie_name with
  | Some _ -> inner_handler request
  | None ->
      let%lwt inner_response = inner_handler request in
      CSRF.add_token inner_response request;
      Lwt.return inner_response

let verify_csrf_cookie inner_handler request =
  match Dream.cookie request CSRF.cookie_name with
  | Some csrf_token -> (
      match%lwt Dream.verify_csrf_token request csrf_token with
      | `Ok -> inner_handler request
      | `Wrong_session | `Expired _ | `Invalid ->
          Dream.respond ~status:`Forbidden "Invalid credentials.")
  | None -> Dream.respond ~status:`Forbidden "Invalid credentials."
