type key
type t

val create : unit -> t
val add : t -> (unit -> unit) -> key
val delete : t -> key -> unit
val forEach : t -> ((unit -> unit) -> unit) -> unit
