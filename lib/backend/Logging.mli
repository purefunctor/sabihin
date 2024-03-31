val src : Logs.src
val msg : Logs.level -> 'a Logs.log
val app : 'a Logs.log
val err : 'a Logs.log
val warn : 'a Logs.log
val info : 'weak2 Logs.log
val debug : 'a Logs.log
val kmsg : (unit -> 'b) -> Logs.level -> ('a, 'b) Logs.msgf -> 'b

val on_error :
  ?level:Logs.level ->
  ?header:string ->
  ?tags:Logs.Tag.set ->
  pp:(Format.formatter -> 'b -> unit) ->
  use:('b -> 'a) ->
  ('a, 'b) result ->
  'a

val on_error_msg :
  ?level:Logs.level ->
  ?header:string ->
  ?tags:Logs.Tag.set ->
  use:(unit -> 'a) ->
  ('a, [ `Msg of string ]) result ->
  'a
