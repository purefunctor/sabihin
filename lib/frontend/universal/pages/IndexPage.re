let indexPageCss = [%cx
  {|
flex-grow: 1;
align-items: center;

$(Theme.afterLarge) {
  display: grid;
  grid-template-rows: 1fr auto;
}
|}
];

[@react.component]
let make = () => {
  <main className=indexPageCss> <IndexPageBanner /> <IndexPageCards /> </main>;
};
