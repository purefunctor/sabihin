module IndexCard = {
  let featureCardCss = [%cx
    {|
    background-color: $(Theme.background9);
    border-radius: 8px;

    box-sizing: border-box;
    padding: 2rem;

    $(Theme.afterMedium) {
      width: calc(50% - 1rem);
    }

    $(Theme.afterLarge) {
      width: initial;
    }
  |}
  ];

  let articleCss = [%cx
    {|
    font-family: "Poppins";
    text-align: center;
  |}
  ];

  let headingCss = [%cx {|
    margin: 0 0 2rem 0;
  |}];

  let iconCss = [%cx
    {|
    color: rgba(0, 0, 0, 0.7);
    background-color: $(Theme.primary);
    padding: 1.5rem;
    border-radius: 100%;
    overflow: visible;
    flex-shrink: 0;
    margin: 0 0 2rem 0;
  |}
  ];

  let pCss = [%cx
    {|
    color: $(Theme.foreground11);
    margin-top: 0.5rem;
    margin-bottom: 0.5rem;
    &:last-child {
      margin-bottom: 0;
    }
  |}
  ];

  [@react.component]
  let make = (~title, ~iconFn, ~children) => {
    <div className=featureCardCss>
      <article className=articleCss>
        {iconFn(~size="3rem", ~className=iconCss)}
        <h1 className=headingCss> {React.string(title)} </h1>
        children
      </article>
    </div>;
  };
};

let cardsCss = [%cx
  {|
display: flex;
flex-direction: column;
gap: 2rem;

padding-top: 4rem;
padding-bottom: 4rem;
border-top: 1px solid $(Theme.background9);

$(Theme.afterMedium) {
  flex-direction: row;
  flex-wrap: wrap;
  justify-content: center;
}

$(Theme.afterLarge) {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
}
|}
];

[@react.component]
let make = () => {
  <section className=cardsCss>
    <IndexCard
      title="End-to-End Encryption"
      iconFn={(~size, ~className) => <Icons.MailLockLine size className />}>
      <p className=IndexCard.pCss>
        {React.string("sabihin.ph utilizes end-to-end encryption techniques.")}
      </p>
      <p className=IndexCard.pCss>
        {React.string("Messages can only be read by its intended recipients.")}
      </p>
    </IndexCard>
    <IndexCard
      title="Open-Source Software"
      iconFn={(~size, ~className) => <Icons.RepositoryLine size className />}>
      <p className=IndexCard.pCss>
        {React.string(
           "sabihin.ph is built as and built with open-source software.",
         )}
      </p>
      <p className=IndexCard.pCss>
        {React.string(
           "Its source code is public and contributions are welcome!",
         )}
      </p>
    </IndexCard>
    <IndexCard
      title="Functional Programming"
      iconFn={(~size, ~className) => <Icons.FunctionVariant size className />}>
      <p className=IndexCard.pCss>
        {React.string(
           "sabihin.ph leverages the power of functional programming languages.",
         )}
      </p>
      <p className=IndexCard.pCss>
        {React.string(
           "OCaml and ReasonML offers performance and type safety.",
         )}
      </p>
    </IndexCard>
  </section>;
};
