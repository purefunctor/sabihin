type t = { id : int32; public_id : string }

let from_request request =
  let ( let* ) = Option.bind in
  let* id = Dream.session_field request "id" in
  let* id = Int32.of_string_opt id in
  let* public_id = Dream.session_field request "public_id" in
  Some { id; public_id }
