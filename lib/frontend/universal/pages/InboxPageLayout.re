let inboxPageCss = [%cx {|
  flex-grow: 1;
|}];

[@react.component]
let make = (~children) => {
  <main className=inboxPageCss> children </main>;
};
