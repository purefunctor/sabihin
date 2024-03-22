module RegisterFields = struct
  type t = { username : string; auth_token : string }

  type e =
    [ `Missing_Field of string
    | `Empty_Field of string
    | `Multiple_Values of string ]

  let render : e -> string = function
    | `Empty_Field field -> Printf.sprintf "Empty field: %s" field
    | `Missing_Field field -> Printf.sprintf "Missing field: %s" field
    | `Multiple_Values field -> Printf.sprintf "Multiple values: %s" field

  module StringMap = Map.Make (String)

  let to_map fields =
    let aux m (k, v) =
      match StringMap.find_opt k m with
      | Some vs -> StringMap.add k (v :: vs) m
      | None -> StringMap.add k [ v ] m
    in
    List.fold_left aux StringMap.empty fields

  let parse (fields : (string * string) list) : (t, [> e ]) result =
    let field_map = to_map fields in
    let find_field field =
      match StringMap.find_opt field field_map with
      | Some [ "" ] -> Error (`Empty_Field field)
      | Some [ value ] -> Ok value
      | Some _ -> Error (`Multiple_Values field)
      | _ -> assert false
    in
    let ( let* ) = Result.bind in
    let* username = find_field "username" in
    let* auth_token = find_field "auth_token" in
    Ok { username; auth_token }
end

let handler request =
  let open RegisterFields in
  match%lwt Dream.form ~csrf:false request with
  | `Ok fields -> (
      match parse fields with
      | Ok _ -> Dream.respond @@ "It's done."
      | Error error -> Dream.respond ~status:`Bad_Request @@ render error)
  | _ -> Dream.respond ~status:`Bad_Request "Invalid request."
