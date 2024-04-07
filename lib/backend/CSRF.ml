let cookie_name = "sabihin.csrf"

let add_token response request =
  (* max_age makes the client aware of the cookie's expiration date,
     which means that refreshing can be used to issue a fresh token. *)
  let valid_for = 60.0 *. 60.0 in
  let max_age = valid_for in
  let fresh_csrf_token = Dream.csrf_token ~valid_for request in
  Dream.set_cookie ~same_site:(Some `Strict) ~max_age response request
    cookie_name fresh_csrf_token
