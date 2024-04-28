let headerCss = [%cx
  {|
font-family: "Josefin Sans";
display: flex;
align-items: center;

border-bottom: 1px solid $(Theme.backgroundSubtle);
padding-top: 1rem;
padding-bottom: 1rem;
|}
];

[@react.component]
let make = () => {
  <header className=headerCss> <HeaderTitle /> <HeaderGetStarted /> </header>;
};
