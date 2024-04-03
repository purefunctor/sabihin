let get_database_url () =
  let options = Toolbox.Database.get_options_from_env () in
  let name =
    match Sys.getenv_opt "SABIHIN_DB_NAME" with
    | Some name -> name
    | None -> "sabihin"
  in
  Backend_lib.Logging.info (fun log -> log "SABIHIN_DB_NAME: %s" name);
  Toolbox.Database.make_url ~options name

let get_server_secret () =
  match Sys.getenv_opt "SABIHIN_SERVER_SECRET" with
  | Some server_secret ->
      Backend_lib.Logging.info (fun log ->
          log "SABIHIN_SERVER_SECRET: Production Mode");
      server_secret
  | None ->
      Backend_lib.Logging.warn (fun log ->
          log "SABIHIN_SERVER_SECRET: Development Mode");
      Dream.to_base64url (Dream.random 128)

let get_cipher_secret () =
  match Sys.getenv_opt "SABIHIN_CIPHER_SECRET" with
  | Some cipher_secret ->
      Backend_lib.Logging.info (fun log ->
          log "SABIHIN_CIPHER_SECRET: Production Mode");
      cipher_secret
  | None ->
      Backend_lib.Logging.warn (fun log ->
          log "SABIHIN_CIPHER_SECRET: Development Mode");
      Dream.to_base64url (Dream.random 128)

let get_live_reload () =
  let has_live_reload =
    Sys.getenv_opt "SABIHIN_LIVE_RELOAD" |> Option.is_some
  in
  if has_live_reload then
    Backend_lib.Logging.info (fun log -> log "SABIHIN_LIVE_RELOAD: Enabled.");
  has_live_reload

let () =
  Dotenv.export ();
  Dream.initialize_log ();

  let database_url = get_database_url () in
  let server_secret = get_server_secret () in
  let cipher_secret = get_cipher_secret () in
  let live_reload = get_live_reload () in

  Backend_lib.Cipher.set_current_key cipher_secret;
  if live_reload then Backend_lib.Dev.enable_live_reload ();
  Backend_lib.Server.run ~database_url ~server_secret ()
