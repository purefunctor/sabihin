let with_json_body request parser handler =
  match Dream.header request "Content-Type" with
  | Some "application/json" -> (
      let%lwt json = Dream.body request in
      let parsed = try Some (parser json) with _ -> None in
      match parsed with
      | Some json -> handler json
      | None -> Dream.respond ~status:`Bad_Request "Invalid request body.")
  | _ -> Dream.respond ~status:`Bad_Request "Invalid content type."
