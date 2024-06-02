let inboxCss = [%cx {|
  flex-grow: 1;
|}];

[@react.component]
let make = (~children) => {
  <main className=inboxCss> children </main>;
};
