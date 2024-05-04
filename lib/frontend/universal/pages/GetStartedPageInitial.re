let initialCss = [%cx
  {|
color: $(Theme.primary);
flex-grow: 1;
display: flex;
align-items: center;
justify-content: center;
|}
];

[@react.component]
let make = () => {
  <div className=initialCss> <Icons.RingResize size="3rem" /> </div>;
};
