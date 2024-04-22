let containerCss = [%cx
  {|
margin-top: 12rem;
margin-bottom: 12rem;
display: flex;
flex-direction: column;
align-items: center;
justify-content: center;
gap: 1rem;
|}
];

let iconCss = [%cx {|
color: $(Theme.primary);
|}];

let labelCss = [%cx {|
margin: 0;
|}];

[@react.component]
let make = (~generate, ~toSubmit) => {
  Js.Console.log2(generate, toSubmit);
  <div className=containerCss>
    <Icons.RingResize size="3rem" className=iconCss />
    <p className=labelCss> {React.string("Loading...")} </p>
  </div>;
};
