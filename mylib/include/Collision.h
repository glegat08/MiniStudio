//#pragma once
//
//#include <iostream>
//#include <vector>
//#include <array>
//#include <type_traits>
//
//class IGameObject;
//
//namespace BT
//{
//    class ICompositeNode;
//
//    enum Status
//    {
//        Success
//        , Failed
//        , Running
//    };
//
//    class RootNode;
//
//    struct IComponentNode
//    {
//        IComponentNode(ICompositeNode* parent);
//
//        void setParent(ICompositeNode* newParent);
//
//        ICompositeNode* getParent();
//        const ICompositeNode* getParent() const;
//
//        virtual ~IComponentNode();
//
//        virtual void display() = 0;
//
//        virtual Status tick() = 0;
//
//        IGameObject* getGameObject();
//        const IGameObject* getGameObject() const;
//
//        RootNode* getRootNode();
//        const RootNode* getRootNode() const;
//
//    protected:
//        ICompositeNode* m_parent;
//    };
//
//    struct IActionNode : IComponentNode
//    {
//        IActionNode(ICompositeNode* node)
//            : IComponentNode(node)
//        {
//        }
//
//        void display() override
//        {
//            std::cout << "child" << std::endl;
//        }
//    };
//
//    class ICompositeNode : public IComponentNode
//    {
//    public:
//        friend IComponentNode;
//
//        ICompositeNode(ICompositeNode* node)
//            : IComponentNode(node)
//        {
//        }
//
//        ~ICompositeNode()
//        {
//            for (std::make_signed_t<size_t> idx = m_children.size() - 1; idx >= 0; --idx)
//            {
//                delete m_children[idx];
//            }
//        }
//
//        void display() override
//        {
//            for (auto& child : m_children)
//                child->display();
//        }
//
//    protected:
//        const std::vector<IComponentNode*>& getChildren() const
//        {
//            return m_children;
//        }
//
//    protected:
//        virtual void add(IComponentNode* node)
//        {
//            m_children.push_back(node);
//        }
//
//        void remove(IComponentNode* node)
//        {
//            auto it = std::find(m_children.begin(), m_children.end(), node);
//            if (it == m_children.end())
//                throw;
//
//            m_children.erase(it);
//        }
//
//    private:
//        std::vector<IComponentNode*> m_children;
//    };
//
//    class ISingleChildNode : public ICompositeNode
//    {
//    public:
//        ISingleChildNode(ICompositeNode* node)
//            : ICompositeNode(node)
//        {
//        }
//
//        bool hasChild() const
//        {
//            return getChildren().size() == 1;
//        }
//
//        IComponentNode* getChild() { return getChildren()[0]; }
//        const IComponentNode* getChild() const { return getChildren()[0]; }
//
//    private:
//        void add(IComponentNode* node) override
//        {
//            if (hasChild())
//                throw;
//
//            ICompositeNode::add(node);
//        }
//    };
//
//    using IDecoratorNode = ISingleChildNode;
//
//    class RootNode : public ISingleChildNode
//    {
//    public:
//        RootNode(IGameObject* boss) : ISingleChildNode(nullptr), m_gameObject(boss)
//        {
//        }
//
//        IGameObject* getGameObject() { return m_gameObject; }
//        const IGameObject* getGameObject() const { return m_gameObject; }
//
//        Status tick() override
//        {
//            if (!hasChild())
//                return Success;
//
//            return getChild()->tick();
//        }
//
//    private:
//        IGameObject* m_gameObject;
//    };
//
//    class IConditionalNode : public IDecoratorNode
//    {
//    public:
//        IConditionalNode(ICompositeNode* node)
//            : IDecoratorNode(node)
//            , m_isTickingChildNode(false)
//        {
//        }
//
//        virtual bool condition() = 0;
//
//        Status tickChildren()
//        {
//            Status childStatus = getChild()->tick();
//            if (childStatus == Success || childStatus == Failed)
//                m_isTickingChildNode = false;
//
//            return childStatus;
//        }
//
//        Status tick() override
//        {
//            if (!m_isTickingChildNode)
//            {
//                bool cond = condition();
//
//                if (!cond)
//                    return Success;
//
//                if (!hasChild())
//                    return Success;
//
//                m_isTickingChildNode = true;
//            }
//
//            return tickChildren();
//        }
//
//    private:
//        bool m_isTickingChildNode;
//    };
//
//    class ControlNode;
//
//    template<Status STATUS>
//    class IStrategy
//    {
//    public:
//        virtual ~IStrategy() = default;
//        virtual Status execute(ControlNode* node) = 0;
//    };
//
//    class ControlNode : public ICompositeNode
//    {
//    public:
//        ControlNode(ICompositeNode* parent, IStrategy<Failed>* failedStrategy, IStrategy<Running>* runningStrategy, IStrategy<Success>* successStrategy)
//            : ICompositeNode(parent)
//            , m_nextNodeToTick(0)
//            , m_failedStrategy(failedStrategy)
//            , m_runningStrategy(runningStrategy)
//            , m_successStrategy(successStrategy)
//        {
//        }
//
//        ~ControlNode()
//        {
//            delete m_failedStrategy;
//            delete m_runningStrategy;
//            delete m_successStrategy;
//        }
//
//        void reset()
//        {
//            m_nextNodeToTick = 0;
//        }
//
//        void incrementChildrenIndex()
//        {
//            m_nextNodeToTick++;
//        }
//
//        bool isLastChildren() const
//        {
//            return m_nextNodeToTick == getChildren().size() - 1;
//        }
//
//        virtual Status tick()
//        {
//            Status childStatus = getChildren()[m_nextNodeToTick]->tick();
//
//            if (childStatus == Failed)
//                return m_failedStrategy->execute(this);
//
//            if (childStatus == Running)
//                return m_runningStrategy->execute(this);
//
//            if (childStatus == Success)
//                return m_successStrategy->execute(this);
//
//            throw;
//        }
//
//    private:
//        size_t m_nextNodeToTick;
//        IStrategy<Failed>* m_failedStrategy;
//        IStrategy<Running>* m_runningStrategy;
//        IStrategy<Success>* m_successStrategy;
//    };
//
//
//
//    struct FailedStrategy : public IStrategy<Failed>
//    {
//        virtual Status execute(ControlNode* node)
//        {
//            node->reset();
//            return Failed;
//        }
//    };
//
//    struct RetryStrategy : public IStrategy<Failed>
//    {
//        virtual Status execute(ControlNode* node)
//        {
//            node->reset();
//            return Running;
//        }
//    };
//
//    struct RunningStrategy : public IStrategy<Running>
//    {
//        virtual Status execute(ControlNode* node)
//        {
//            return Running;
//        }
//    };
//
//    struct SuccessStrategy : public IStrategy<Success>
//    {
//        virtual Status execute(ControlNode* node)
//        {
//            if (node->isLastChildren())
//            {
//                node->reset();
//                return Success;
//            }
//
//            node->incrementChildrenIndex();
//            return Running;
//        }
//    };
//
//    struct RedoNTimeStrategy : public IStrategy<Success>
//    {
//        RedoNTimeStrategy(int N) : m_N(N)
//        {
//        }
//
//        virtual Status execute(ControlNode* node)
//        {
//            if (node->isLastChildren())
//            {
//                node->reset();
//                m_N--;
//                if (m_N == 0)
//                    return Success;
//
//                return Running;
//            }
//
//            node->incrementChildrenIndex();
//            return Running;
//        }
//
//    private:
//        int m_N;
//    };
//
//    struct RedoStrategy : public IStrategy<Success>
//    {
//        virtual Status execute(ControlNode* node)
//        {
//            if (node->isLastChildren())
//            {
//                node->reset();
//                return Running;
//            }
//
//            node->incrementChildrenIndex();
//            return Running;
//        }
//    };
//
//    struct FailAsSuccessStrategy : public IStrategy<Failed>
//    {
//        virtual Status execute(ControlNode* node)
//        {
//            node->reset();
//            return Success;
//        }
//    };
//
//    class DoNTime : public ControlNode
//    {
//    public:
//        DoNTime(ICompositeNode* parent, int N = 3) :
//            ControlNode(parent, new FailedStrategy, new RunningStrategy, new RedoNTimeStrategy{ N })
//        {
//        }
//    };
//
//    class DoUntilFailure : public ControlNode
//    {
//    public:
//        DoUntilFailure(ICompositeNode* parent) :
//            ControlNode(parent, new FailAsSuccessStrategy, new RunningStrategy, new RedoStrategy)
//        {
//        }
//    };
//
//    class Sequence : public ControlNode
//    {
//    public:
//        Sequence(ICompositeNode* parent) :
//            ControlNode(parent, new FailedStrategy, new RunningStrategy, new SuccessStrategy)
//        {
//        }
//    };
//
//    class Retry : public ControlNode
//    {
//    public:
//        Retry(ICompositeNode* parent) :
//            ControlNode(parent, new RetryStrategy, new RunningStrategy, new SuccessStrategy)
//        {
//        }
//    };
//
//    template<typename GameObjectType, typename NodeType>
//    class BehaviorNodeDecorator : public NodeType
//    {
//    public:
//        BehaviorNodeDecorator(ICompositeNode* parent) : NodeType(parent)
//        {
//        }
//
//        //virtual bool condition()
//        //{
//           // 
//        //}
//
//        GameObjectType* getGameObject() { return static_cast<GameObjectType*>(NodeType::getGameObject()); }
//        const GameObjectType* getGameObject() const { return static_cast<const GameObjectType*>(NodeType::getGameObject()); }
//    };
//
//}