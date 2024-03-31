let get_database_url () =
  match Sys.getenv_opt "DATABASE_URL" with
  | Some url ->
      Backend_lib.Logging.info (fun log -> log "DATABASE_URL: Development Mode");
      url
  | None ->
      Backend_lib.Logging.warn (fun log -> log "DATABASE_URL: Development Mode");
      "postgresql://localhost:5432/sabihin"

let get_server_secret () =
  match Sys.getenv_opt "SERVER_SECRET" with
  | Some server_secret ->
      Backend_lib.Logging.info (fun log -> log "SERVER_SECRET: Production Mode");
      server_secret
  | None ->
      Backend_lib.Logging.warn (fun log ->
          log "SERVER_SECRET: Development Mode");
      Dream.to_base64url (Dream.random 128)

let () =
  Dream.initialize_log ();
  let database_url = get_database_url () in
  let server_secret = get_server_secret () in
  Backend_lib.Server.run ~database_url ~server_secret ()
