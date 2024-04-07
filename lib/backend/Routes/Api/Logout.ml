let handler request =
  Dream.invalidate_session request;%lwt
  let response = Dream.response ~status:`No_Content "" in
  CSRF.add_token response request;
  Lwt.return response
