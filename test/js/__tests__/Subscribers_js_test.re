open Jest;
open Expect;

describe("API", () => {
  test("it works", () => {
    module S = Subscribers_js;
    let subscribers = S.create();
    let first = ref(false);
    let second = ref(false);
    let third = ref(false);
    let _ = S.add(subscribers, () => {first := true});
    let _ = S.add(subscribers, () => {second := true});
    let third_key = S.add(subscribers, () => {second := true});
    S.delete(subscribers, third_key);
    S.forEach(subscribers, callback => {callback()});
    expect([|first^, second^, third^|]) |> toEqual([|true, true, false|]);
  })
});
