module Internal = struct
  type t

  external create : unit -> t = "Map" [@@mel.new]
  external add : t -> int -> (unit -> unit) -> unit = "set" [@@mel.send]
  external delete : t -> int -> unit = "delete" [@@mel.send]

  external forEach : t -> (((unit -> unit) -> int -> t -> unit)[@u]) -> unit
    = "forEach"
  [@@mel.send]
end

type key = int
type t = Internal.t

let current_key = ref 0
let create = Internal.create

let add subscribers subscriber =
  let key = !current_key in
  Internal.add subscribers key subscriber;
  current_key := !current_key + 1;
  key

let delete = Internal.delete

let forEach subscribers callback =
  Internal.forEach subscribers (fun [@u] subscriber _ _ -> callback subscriber)
