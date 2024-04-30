let getStartedCss = [%cx
  {|
flex-grow: 1;
display: flex;
flex-direction: column;
font-family: "Poppins";
margin-top: 2rem;
margin-bottom: 2rem;

$(Theme.afterMedium) {
  align-self: center;
  width: 100%;
  max-width: 768px;
}
|}
];

[@react.component]
let make = (~children) => {
  <main className=getStartedCss> children </main>;
};
