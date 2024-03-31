let database_url =
  match Sys.getenv_opt "DATABASE_URL" with
  | Some url ->
      Dream.info (fun log -> log "DATABASE_URL: Production Mode");
      url
  | None ->
      Dream.warning (fun log -> log "DATABASE_URL: Development Mode");
      "postgresql://localhost:5432/sabihin"

let server_secret =
  match Sys.getenv_opt "SERVER_SECRET" with
  | Some server_secret ->
      Dream.info (fun log -> log "SERVER_SECRET: Production Mode");
      server_secret
  | None ->
      Dream.warning (fun log -> log "SERVER_SECRET: Development Mode");
      Dream.to_base64url (Dream.random 128)

let () = Backend_lib.Server.run ~database_url ~server_secret ()
