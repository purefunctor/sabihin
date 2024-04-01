type database_options = {
  credentials : (string * string) option;
  host : string;
  port : string;
}

let get_options_from_env () =
  let ( let* ) = Option.bind in
  let credentials =
    let* username = Sys.getenv_opt "SABIHIN_DB_USERNAME" in
    let* password = Sys.getenv_opt "SABIHIN_DB_PASSWORD" in
    Some (username, password)
  in
  let host =
    Sys.getenv_opt "SABIHIN_DB_HOST" |> Option.value ~default:"localhost"
  in
  let port = Sys.getenv_opt "SABIHIN_DB_PORT" |> Option.value ~default:"5432" in
  { credentials; host; port }

let make_url ~options =
  let { credentials; host; port } = options in
  match credentials with
  | Some (username, password) ->
      Printf.sprintf "postgresql://%s:%s@%s:%s/%s" username password host port
  | None -> Printf.sprintf "postgresql://%s:%s/%s" host port
