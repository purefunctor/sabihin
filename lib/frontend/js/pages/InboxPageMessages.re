let layoutCss = [%cx
  {|
  display: flex;
  padding-top: 2rem;
  padding-bottom: 2rem;
  gap: 2rem;
  flex-direction: column;
|}
];

[@react.component]
let make = () => {
  <div className=layoutCss>
    <InboxPageMessagesCard />
    <InboxPageMessagesCard />
  </div>;
};
