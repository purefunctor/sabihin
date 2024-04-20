let dev_var = ref(false);

let is_dev = () => dev_var^;

let enable_dev = () => dev_var := true;

let make_script_tags_from_manifest = () => {
  open Yojson.Safe;

  let manifest_json = from_file("./dist/.vite/manifest.json");
  let index_js =
    manifest_json
    |> Util.member("index.js")
    |> Util.member("file")
    |> Util.to_string
    |> Printf.sprintf("/%s");

  <script type_="module" src=index_js />;
};

let make_script_tags = () => {
  is_dev()
    ? <>
        <script type_="module" src="http://localhost:5173/@vite/client" />
        <script type_="module" src="http://localhost:5173/index.js" />
      </>
    : make_script_tags_from_manifest();
};

let get_static_path = () => {
  is_dev() ? "public" : "dist";
};
