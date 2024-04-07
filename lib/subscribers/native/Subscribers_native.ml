module Internal = Map.Make (Int)

type key = int
type t = (unit -> unit) Internal.t ref

let current_key = ref 0
let create () = ref Internal.empty

let add subscribers subscriber =
  let key = !current_key in
  subscribers := Internal.add key subscriber !subscribers;
  current_key := !current_key + 1;
  key

let delete subscribers key = subscribers := Internal.remove key !subscribers

let for_each subscribers callback =
  Internal.iter (fun _ subscriber -> callback subscriber) !subscribers
