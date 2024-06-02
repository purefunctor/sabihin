open InboxPageMessagesStyles;

[@react.component]
let make = () => {
  <div className=messagesCss>
    <InboxPageMessagesCard />
    <InboxPageMessagesCard />
    <InboxPageMessagesCard />
  </div>;
};
